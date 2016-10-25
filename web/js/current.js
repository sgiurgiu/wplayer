wplayerAppControllers.controller('CurrentPlayingController', function($scope,$http,$log) {
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

    $scope.playFile = function(file) {
        $log.log('playing file '+file.name);
        $http.post('/api/play_movie/'+file.link);
    };
    

    $scope.ws=new WebSocket("ws://localhost:9090/player/");
    $scope.ws.onmessage=function(evt) {
        console.log("received:"+evt.data);        
    };
    
    $scope.ws.onopen=function(evt){
        console.log("openend connection");
    };
    $scope.send = function() {
        $scope.ws.send("Hello");        
    };
    $scope.close = function() {
        $scope.ws.close();
    };

        $log.log('Loading files');
	$scope.loadFiles('');
});
