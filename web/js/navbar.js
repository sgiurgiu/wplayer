angular.module('navbar', []);

angular.module('navbar').component('navbar', {
    templateUrl:'navbar.html',
    controller:['$log','$timeout','$location',function($log,$timeout,$location) {
        var self = this;
        self.tabs = [];
        self.setActive = function (tab) {
            $log.log('tab active:'+tab);
            self.tabs= [];
            self.tabs[tab]='active';
        };
        var path = $location.path();
        $log.log('path:'+path);
        var indexOfSlash = path.indexOf('/',1);
        if(indexOfSlash < 0)
        {
            indexOfSlash = (function () { return; })();
        }
        var tab = path.substring(1,indexOfSlash);
        $log.log('activating tab:'+tab);
        self.setActive(tab);
    }
        
    ]
});
 



