angular.module('movies', ['files.service']);

angular.module('movies').component('movies', {
    templateUrl:'movies.html',
    controller:['$routeParams','$log','$location','Files',  function($routeParams,$log,$location,Files) {
        var self = this;
	self.files_list = [];

	self.loadFiles = function(path) {
            
            self.files_list = [];
            
            Files.list({'path':path},path,function(data){
                self.cur_dir = data.cur_dir;
                self.files_list = data.files;
                $log.log('got files '+data);                
            });
	};

        $log.log('route params:'+JSON.stringify($routeParams.path));
        $log.log('location :'+JSON.stringify($location));
	self.loadFiles($routeParams.path);
    }
        
    ]
                                       
});
 

