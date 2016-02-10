wplayerAppControllers.controller('CurrentPlayingController', function($scope,$http,$log) {
	$scope.files_list = [];

	$scope.loadFiles = function() {
		$http.get('/api/files')
			.success(function(data){
				$scope.files_list = data;
				$log.log('got files '+data);
			})
			.error(function(response){

			});
	};

	$log.log('Loading files');
	$scope.loadFiles();	
});