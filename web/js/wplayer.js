'use strict';

var wplayerApp = angular.module('wplayer', ['ui.bootstrap','ngRoute','ngAnimate','current','movies','custom','files.service']);

wplayerApp.config (['$routeProvider','$locationProvider',function config($routeProvider,$locationProvider){
                      //  $locationProvider.html5Mode(true);
                        $locationProvider.hashPrefix('!');

                        $routeProvider
                        .when('/current',{
                                template : '<current></current>'
                        }).when('/movies/:path*',{
                                template : '<movies></movies>'                       
                        }).when('/movies',{
                                template : '<movies></movies>'                        
                        }).when('/custom',{
                                template : '<custom></custom>'
                        }).otherwise('/movies');
                }
        ]);

wplayerApp.filter('bytes', function() {
    return function(bytes, precision) {
        if (isNaN(parseFloat(bytes)) || !isFinite(bytes)) return '';
        if (typeof precision === 'undefined') precision = 1;
        var units = ['B', 'KB', 'MB', 'GB', 'TB', 'PB'],
            number = Math.floor(Math.log(bytes) / Math.log(1024));
        return (bytes / Math.pow(1024, Math.floor(number))).toFixed(precision) +  ' ' + units[number];
    }
});
wplayerApp.filter('duration', function() {
    return function(seconds) {
        if (isNaN(parseInt(seconds)) || !isFinite(seconds)) return '';
        var mom = moment.utc(seconds*1000);
        return mom.format('HH:mm:ss');
    }
});

wplayerApp.run(function($rootScope,$location,$log) {
    $rootScope.ws=new WebSocket("ws://"+$location.host()+":"+$location.port()+"/player");
    $rootScope.playFile = function(file) {
        $log.log('playing file '+file.name);
        $rootScope.ws.send(JSON.stringify({name:'play',link:file.link}));      
    };
    $rootScope.current_movie = {};
    $rootScope.ws.onmessage=function(evt) {
        //$log.log("received:"+evt.data);        
        $rootScope.$apply(function(){
            $rootScope.current_movie = JSON.parse(evt.data);
            //$log.log("$scope.current_movie:"+$rootScope.current_movie);
            //$log.log("$scope.current_movie:"+$rootScope.current_movie.file_name);                            
        });
    };
      
    $rootScope.ws.onopen=function(evt){
        console.log("openend connection");
    };
    
});

