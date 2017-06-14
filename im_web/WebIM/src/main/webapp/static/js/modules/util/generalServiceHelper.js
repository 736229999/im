define(function (require, exports, module) {
    require('$');

    var serviceHelper = {
        config: {
            defaultJsonPConfig: {
                async: true,
                dataType: 'jsonp',
                jsonp: 'callback',
                type: 'get'
            }
        },
        callJsonPService: function (options) {
            options = $.extend(this.config.defaultJsonPConfig, options);

            $.ajax(options);
        }
    };

    module.exports = serviceHelper;
});