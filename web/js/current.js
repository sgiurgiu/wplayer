wplayerAppControllers.controller('CurrentPlayingController', function($scope,$log) {
	
    $scope.props = {
            vol_value :0,
            enable_seek:false
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
        $scope.ws.send(JSON.stringify({name:'volume',value:$scope.current_movie.volume}));      
    };
    $scope.backward = function() {
        $scope.ws.send(JSON.stringify({name:'backward'}));      
    };
    $scope.forward = function() {
        $scope.ws.send(JSON.stringify({name:'forward'}));      
    };
    $scope.fastBackward = function() {
        $scope.ws.send(JSON.stringify({name:'fast-backward'}));      
    };
    $scope.fastForward = function() {
        $scope.ws.send(JSON.stringify({name:'fast-forward'}));      
    };
    $scope.seekPercent = function(e) {
        if(!$scope.props.enable_seek) return;
        var fullProgressBarWidth = $(e.currentTarget).width();
        var requestedPosition = e.offsetX / fullProgressBarWidth * 100;        
        $log.log('seek percent: '+requestedPosition);
        $scope.ws.send(JSON.stringify({name:'seek-percent',value:requestedPosition}));      
    };
            
    $log.log("$scope.current_movie:"+$scope.current_movie);    
      
    
});
