angular.module('playlist', ['player.service']);

angular.module('playlist').component('playlist', {
    templateUrl:'playlist.html',
    controller:['$log','$timeout','$scope','Player',  function($log,$timeout,$scope,Player) {
        var self = this;
        self.current_playlist = [];
        $scope.$watch(function(){return Player.getPlaylist();},function(newValue,oldValue){
            $log.log('self$wattch current getPlaylist '+JSON.stringify(newValue));
            self.current_playlist = newValue;
        });
        $scope.$watch(function(){return Player.connectionOpened();},function(newValue,oldValue){
            $log.log('self$wattch connectionOpened '+JSON.stringify(newValue));
            if(newValue) {            
                Player.getPlaylistFromServer();
            }
        });
    }
        
    ]
});
 


