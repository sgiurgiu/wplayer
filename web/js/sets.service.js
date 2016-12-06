'use strict';
angular.module('sets.service',['ngResource']);

angular.module('sets.service').factory('Sets',['$resource', function($resource) {
    return $resource('api/sets/:name',{},{
    });
}
]);

