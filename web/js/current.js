angular.module('current',['player.service']);

angular.module('current').component('current',{
    templateUrl:'current.html',
    controller: ['Player','$log','$scope',function(Player,$log,$scope) {
    
        var self = this;
        self.current_movie = {};
        self.current_movie_paused = false;
        
        $scope.$watch(function(){return Player.getCurrentMovie();},function(newValue,oldValue){
            $log.log('self$wattch current movie '+JSON.stringify(newValue));
                self.current_movie = newValue;
        });
        
        self.props = {
            
                enable_seek:false
        };
        
        self.stopPlay = function() {        
            Player.stopPlay();
        };
        
        self.pausePlay = function() {        
            Player.pausePlay();
        };
        self.resumePlay = function() {        
            Player.resumePlay();
        };
        self.pauseResumePlay = function() {
            self.current_movie_paused  = ! self.current_movie_paused;
            Player.pauseResumePlay(self.current_movie_paused);
        };
        
        self.setVolume = function (){
            
            Player.setVolume(self.current_movie.volume);
        };
        self.backward = function() {
            Player.backward();
        };
        self.forward = function() {
            Player.forward();
        };
        self.fastBackward = function() {
            Player.fastBackward();
        };
        self.fastForward = function() {
            Player.fastForward();
        };
        self.seekPercent = function(e) {
            if(!self.props.enable_seek) return;
            var fullProgressBarWidth = $(e.currentTarget).width();
            var requestedPosition = e.offsetX / fullProgressBarWidth * 100;        
            $log.log('seek percent: '+requestedPosition);
            Player.seekPercent(requestedPosition);
        };
    
}]
});
                                    
                                    
