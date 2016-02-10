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

var wplayerAppControllers = angular.module('wplayerAppControllers',[]);

wplayerAppControllers.controller('WPlayerController',function($scope,$route){
	$scope.$route = $route;	
});
