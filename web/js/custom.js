angular.module('custom',['player.service']);

angular.module('custom').component('custom',{
    templateUrl:'custom.html',
    controller: ['$log','Player',function($log,Player) {
        
        var self = this;
        
	self.youtube_id = "";

	self.playYoutube = function(id) {
            $log.log('youtube id:'+id);
            Player.playYoutube(id);            
	};

}]
});




