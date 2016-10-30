wplayerAppControllers.controller('MoviesListingController', function($scope,$http,$log) {
	$scope.files_list = [];

	$scope.loadFiles = function(path) {
                var url = '/api/files';
                if(path !== '')  url+= '/'+path;
		$http.get(url)
			.success(function(data){
                                $scope.cur_dir = data.cur_dir;
				$scope.files_list = data.files;
				$log.log('got files '+data);
			})
			.error(function(response){

			});
	};

        $log.log('Loading files');
	$scope.loadFiles('');
});

