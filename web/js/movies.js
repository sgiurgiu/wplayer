angular.module('movies', ['files.service','player.service']);

angular.module('movies').component('movies', {
    templateUrl:'movies.html',
    controller:['$routeParams','$log','$location','$timeout','Files','Player',  function($routeParams,$log,$location,$timeout,Files,Player) {
        var self = this;
        self.loading = true;
        self.message = null;
        self.files_list = [];
        self.original_file_list=[];
        self.highlightedFile = null;
        Files.list({'path':$routeParams.path},function(data){
            self.loading = false;
            self.cur_dir = data.cur_dir;
            self.files_list = data.files;
            self.original_file_list = JSON.parse(JSON.stringify(self.files_list));
            $log.log('got files '+data);                
        }, function(error){
            self.loading = false;   
        });
        
        self.addToPlaylist = function(file) {
            Player.addToPlaylist(file);
            self.message = "Added to playlist";
            $timeout(function(){
                self.message = null;
            },5000);
        };
        self.addToPlaylistPlay = function(file) {
            Player.addToPlaylistPlay(file);
            self.message = "Added to playlist and started playing";
            $timeout(function(){
                self.message = null;
            },5000);            
        };        
        self.playFile = function(file) {
            Player.playFile(file);
            self.message = "Playing file";
            $timeout(function(){
                self.message = null;
            },5000);
        };
        self.closeMessage = function(){
            self.message = null;
        };
        self.getRowClass = function(file) {
            return self.highlightedFile === file ? "active" : "";
        };
        self.fileHighlighted = function(file) {
            self.highlightedFile = file;
        };
        
        self.searchText = "";
    }
        
    ]
                                       
});
 

