var wplayerApp = angular.module('wplayer', ['ui.bootstrap','ngRoute','ngAnimate','wplayerAppControllers']);

wplayerApp.config (['$routeProvider',function($routeProvider){
                        $routeProvider.when('/current',{
                                templateUrl : '/current.html',
                                controller: 'CurrentPlayingController',
                                controllerAs: 'current'
                        }).when('/movies',{
                                templateUrl : '/movies.html',
                                controller: 'MoviesListController',
                                controllerAs: 'movielist'
                        }).otherwise({ redirectTo: '/current' });
                }
        ]);

wplayerApp.config (['$locationProvider',function($locationProvider){
                        $locationProvider.html5Mode(true);
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

var wplayerAppControllers = angular.module('wplayerAppControllers',[]);

wplayerAppControllers.controller('WPlayerController',function($scope,$route){
	$scope.$route = $route;	
});
