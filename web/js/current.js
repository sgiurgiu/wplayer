wplayerAppControllers.controller('CurrentPlayingController', function($scope,$http,$log) {
	$scope.files_list = [];

	$scope.loadFiles = function(path) {
                var url = '/api/files';
                if(path !== '')  url+= '/'+path;
		$http.get(url)
			.success(function(data){
				$scope.files_list = data;
				$log.log('got files '+data);
			})
			.error(function(response){

			});
	};

    $scope.playFile = function(file) {
        $log.log('playing file '+file.name);
        $http.post('/api/play_movie/'+file.link);
    };

	$log.log('Loading files');
	$scope.loadFiles('');
});
