'use strict';
angular.module('files.service',['ngResource']);

angular.module('files.service').factory('Files',['$resource', function($resource) {
    return $resource('api/files/:path',{},{
        'list' : {
            method:'GET',
            params: {},
            isArray:false
        }
    });
}
]);
