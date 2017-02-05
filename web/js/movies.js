angular.module('movies', ['files.service','player.service']);

angular.module('movies').component('movies', {
    templateUrl:'movies.html',
    controller:['$routeParams','$log','$location','$timeout','Files','Player',  function($routeParams,$log,$location,$timeout,Files,Player) {
        var self = this;
        self.loading = true;
        self.playStartedSuccess = false;
	self.files_list = [];
        self.original_file_list=[];
        Files.list({'path':$routeParams.path},function(data){
            self.loading = false;
            self.cur_dir = data.cur_dir;
            self.files_list = data.files;
            self.original_file_list = JSON.parse(JSON.stringify(self.files_list));
            $log.log('got files '+data);                
        }, function(error){
            self.loading = false;   
        });

	self.playFile = function(file) {
            Player.playFile(file);
            self.playStartedSuccess = true;
            $timeout(function(){
                self.playStartedSuccess = false;
            },5000);
        };
        self.closeMessage = function(){
            self.playStartedSuccess = false;
        };
        
        self.searchText = "";
    }
        
    ]
                                       
});
 

