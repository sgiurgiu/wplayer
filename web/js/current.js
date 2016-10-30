wplayerAppControllers.controller('CurrentPlayingController', function($scope,$log) {
	
    
    $scope.stopPlay = function() {        
        $scope.ws.send(JSON.stringify({name:'stop'}));      
    };
    
    $log.log("$scope.current_movie:"+$scope.current_movie);    
      
    
});
