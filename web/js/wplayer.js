'use strict';

var wplayerApp = angular.module('wplayer', ['ui.bootstrap','ngRoute','ngAnimate','current','movies','custom','settings','playlist','navbar']);

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
                        }).when('/settings',{
                                template : '<settings></settings>'
                        }).when('/playlist',{
                                template : '<playlist></playlist>'
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
wplayerApp.filter('fileName', function() {
    return function(file) {
        var lastIndex = file.lastIndexOf('/');
        if(lastIndex >= 0)
        {
            return file.substring(lastIndex+1);
        }
        return file;
    }
});
