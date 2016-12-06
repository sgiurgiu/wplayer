angular.module('settings_multimedia_sets',['sets.service']);

angular.module('settings_multimedia_sets').component('settingsMultimediaSets',{
    templateUrl:'settings_multimedia_sets.html',
    controller: ['$log','Sets',function($log,Sets) {
        
        var self = this;
        self.new_set = {};
        self.sets = Sets.query();

        self.addSet = function(){
            Sets.save(self.new_set,function(){
                self.sets = Sets.query();
                self.new_set = {};
            });
        };
        self.deleteSet = function(set) {
            Sets.remove({'name':set.name},function(){
                self.sets = Sets.query();
            });
        };
}]
});


