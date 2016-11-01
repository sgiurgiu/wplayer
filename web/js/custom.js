wplayerAppControllers.controller('CustomPlayingController', function($scope,$log) {
	$scope.youtube_id = "";

	$scope.playYoutube = function(id) {
            $log.log('youtube id:'+id);
            $scope.ws.send(JSON.stringify({name:'youtube',value:id}));      
	};

});


