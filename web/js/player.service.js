'use strict';
angular.module('player.service',[]);

angular.module('player.service').factory('Player',['$location','$log','$rootScope', function($location,$log,$rootScope) {
    var self = this;
    self.current_movie = {};
    self.current_playlist = [];
    self.connection_opened = {value:false};
    
    var service = {
        connectionOpened: function() {
            return self.connection_opened.value;
        },
        setCurrentMovie : function(movie) {
            self.current_movie = movie;
        },
        getCurrentMovie: function() {
            return self.current_movie;
        },
        setPlaylist : function(playlist) {
            self.current_playlist = playlist;
        },
        getPlaylist : function() {            
            return self.current_playlist;
        },
        getPlaylistFromServer : function() {
            self.ws.send(JSON.stringify({name:'playlist'}));
        },
        addToPlaylist : function(file) {
            self.ws.send(JSON.stringify({name:'playlist-add',link:file.link}));      
        },
        addToPlaylistPlay : function(file) {
            self.ws.send(JSON.stringify({name:'playlist-add-play',link:file.link}));      
        },
        playlistNext : function() {
            self.ws.send(JSON.stringify({name:'playlist-next'}));
        },
        playlistPrevious : function() {
            self.ws.send(JSON.stringify({name:'playlist-previous'}));
        },
        playlistClear : function() {
            self.ws.send(JSON.stringify({name:'playlist-clear'}));
        },
        playlistShuffle : function() {
            self.ws.send(JSON.stringify({name:'playlist-shuffle'}));
        },
        playlistRemove : function(index) {
            self.ws.send(JSON.stringify({name:'playlist-remove',value:index}));
        },
        playlistMove : function(index1,index2) {
            self.ws.send(JSON.stringify({name:'playlist-move',value1:index1,value2:index2}));
        },
        playFile : function(file) {
            $log.log('playing file '+file.name);
            self.ws.send(JSON.stringify({name:'play',link:file.link}));      
        },
        stopPlay : function() {        
            self.ws.send(JSON.stringify({name:'stop'}));
        },
        pausePlay : function() {        
            self.ws.send(JSON.stringify({name:'pause'}));      
        },
        resumePlay : function() {        
            self.ws.send(JSON.stringify({name:'resume'}));      
        },
        pauseResumePlay : function(pause) {            
            self.ws.send(JSON.stringify({name:pause?'pause':'resume'}));              
        },        
        setVolume : function (volume){
            $log.log('volume:'+volume);
            self.ws.send(JSON.stringify({name:'volume',value:volume}));      
        },
        backward : function() {
            self.ws.send(JSON.stringify({name:'backward'}));      
        },
        forward : function() {
            self.ws.send(JSON.stringify({name:'forward'}));      
        },
        fastBackward : function() {
            self.ws.send(JSON.stringify({name:'fast-backward'}));      
        },
        fastForward : function() {
            self.ws.send(JSON.stringify({name:'fast-forward'}));      
        },
        seekPercent : function(percent) {
            self.ws.send(JSON.stringify({name:'seek-percent',value:percent}));      
        },
        playYoutube : function(id) {
                self.ws.send(JSON.stringify({name:'youtube',value:id}));
        },
        removeSub : function(id) {
            self.ws.send(JSON.stringify({name:'remove-sub',value:id}));
        }
        
    };
    
    self.ws=new WebSocket("ws://"+$location.host()+":"+$location.port()+"/player");    
    self.ws.onmessage=function(evt) {
        $log.log("received:"+evt.data);        
        $rootScope.$apply(function() {
            var data = JSON.parse(evt.data);
            if( Object.prototype.toString.call( data ) === '[object Array]' ) {
                //we got playlist
                service.setPlaylist(data);
            } else {
                service.setCurrentMovie(data);
                $log.log("$scope.current_movie:"+JSON.stringify(service.getCurrentMovie()));
            }
        });
    };
      
    self.ws.onopen=function(evt){
        console.log("openend connection");
        self.connection_opened.value = true;
    };
    
    return service;
    
}
]);

