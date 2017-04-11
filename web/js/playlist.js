angular.module('playlist', ['player.service']);

angular.module('playlist').component('playlist', {
    templateUrl:'playlist.html',
    controller:['$log','$timeout','$scope','Player',  function($log,$timeout,$scope,Player) {
        var self = this;
        self.current_playlist = [];
        self.highlightedFile = null;
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
         self.fileHighlighted = function(file) {
            self.highlightedFile = file;
        };
        self.shuffle = function() {
            Player.playlistShuffle();
            Player.getPlaylistFromServer();
        };
        self.clear = function() {
            Player.playlistClear();
            Player.getPlaylistFromServer();
        };
        self.playPrevious = function() {
            Player.playlistPrevious();
            Player.getPlaylistFromServer();            
        };
        self.playNext = function() {
            Player.playlistNext();
            Player.getPlaylistFromServer();            
        };
        self.moveUp = function(index) {
            if(index <= 0) return;
            Player.playlistMove(index,index-1);
            Player.getPlaylistFromServer();            
        };
        self.moveDown = function(index) {
            if(index >= (self.current_playlist.length -1)) return;
            Player.playlistMove(index,index+1);
            Player.getPlaylistFromServer();            
        };
        self.delete = function(index) {
            Player.playlistRemove(index);
            Player.getPlaylistFromServer();                        
        };
    }
        
    ]
});
 


