'use strict';
angular.module('player.service',[]);

angular.module('player.service').factory('Player',['$location','$log','$rootScope', function($location,$log,$rootScope) {
    var self = this;
    self.current_movie = {};
    
    var service = {
        setCurrentMovie : function(movie) {
            self.current_movie = movie;
        },
        getCurrentMovie: function() {
            return self.current_movie;
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
	}        
        
    };
    
    self.ws=new WebSocket("ws://"+$location.host()+":"+$location.port()+"/player");    
    
    self.ws.onmessage=function(evt) {
        //$log.log("received:"+evt.data);        
        $rootScope.$apply(function(){
            service.setCurrentMovie(JSON.parse(evt.data));
            $log.log("$scope.current_movie:"+self.current_movie);
            $log.log("$scope.current_movie:"+self.current_movie.file_name);                            
        });
    };
      
    self.ws.onopen=function(evt){
        console.log("openend connection");
    };
    
    return service;
    
}
]);

