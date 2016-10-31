wplayerAppControllers.controller('CurrentPlayingController', function($scope,$log) {
	
    $scope.props = {
            vol_value :0
    };
    
    $scope.stopPlay = function() {        
        $scope.ws.send(JSON.stringify({name:'stop'}));      
    };
    
    $scope.pausePlay = function() {        
        $scope.ws.send(JSON.stringify({name:'pause'}));      
    };
    $scope.resumePlay = function() {        
        $scope.ws.send(JSON.stringify({name:'resume'}));      
    };
    $scope.pauseResumePlay = function() {
        $scope.current_movie.paused  = !$scope.current_movie.paused;
        $scope.ws.send(JSON.stringify({name:$scope.current_movie.paused?'pause':'resume'}));              
    };
    
    $scope.setVolume = function (){
        $log.log('volume:'+$scope.props.vol_value);
        $scope.ws.send(JSON.stringify({name:'volume',value:$scope.props.vol_value}));      
    };
            
    $log.log("$scope.current_movie:"+$scope.current_movie);    
      
    
});
