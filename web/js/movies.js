angular.module('movies', ['files.service','player.service']);

angular.module('movies').component('movies', {
    templateUrl:'movies.html',
    controller:['$routeParams','$log','$location','Files','Player',  function($routeParams,$log,$location,Files,Player) {
        var self = this;
        self.loading = true;
	self.files_list = [];
        Files.list({'path':$routeParams.path},function(data){
            self.loading = false;
            self.cur_dir = data.cur_dir;
            self.files_list = data.files;
            $log.log('got files '+data);                
        });

	self.playFile = function(file) {
            Player.playFile(file);
        };
    }
        
    ]
                                       
});
 

