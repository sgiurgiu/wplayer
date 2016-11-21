angular.module('movies', ['files.service']);

angular.module('movies').component('movies', {
    templateUrl:'movies.html',
    controller:['$routeParams','$http','$log','$location','Files','$timeout','$route',  function($routeParams,$http,$log,$location,Files,$timeout,$route) {
        var self = this;
	self.files_list = [];

	self.loadFiles = function(path) {
            
            self.files_list = [];
            
            Files.list({'path':path},function(data){
                self.cur_dir = data.cur_dir;
                self.files_list = data.files;
                $log.log('got files '+data);                
            });
            
            
            /*var url = '/api/files';
            if(path !== null && path.length>0 && path[0]==='!') path = path.substr(1);
            if(path !== '' && path !== null)  url+= '/'+path;
                                   
            $http.get(url)
                    .success(function(data){
                            self.cur_dir = data.cur_dir;
                            self.files_list = data.files;
                            $log.log('got files '+data);
                    })
                    .error(function(response){

                    });*/
	};

        $log.log('route params:'+JSON.stringify($routeParams));
       // $log.log('route params:'+JSON.stringify($route));
        
        $log.log('location :'+JSON.stringify($location));
	self.loadFiles($location.hash());
        $timeout(function(){
      // do stuff with $routeParams
            $log.log('routeParams:'+JSON.stringify($routeParams));
        }, 20);
    }
        
    ]
                                       
});
 

