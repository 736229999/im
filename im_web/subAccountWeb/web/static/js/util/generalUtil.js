/**
 * Created by wangchen on 2015/2/6.
 */
var generalUtil = function () {
    var _today;

    return {
        _defaultDateSeparator: '-',
        _defaultTimeSeparator: ':',

        defaultDate: function () {
            return _today || (_today = this.formateDate(new Date()));
        },
        formateDate: function (date) {
            return date.getFullYear() + this._defaultDateSeparator
                + this.zeroFilled(1 + date.getMonth()) + this._defaultDateSeparator
                + this.zeroFilled(date.getDate());
        },
        formateFullTime: function(date) {

        },
        formateOnlyTime: function(date) {
            return this.zeroFilled(date.getHours()) + this._defaultTimeSeparator
                + this.zeroFilled(date.getMinutes()) + this._defaultTimeSeparator
                + this.zeroFilled(date.getSeconds());
        },
        zeroFilled: function (i) {
            return i < 10 ? '0' + i : i;
        },
        parseDate: function (dateString) {
            var isoExp = /^\s*(\d{4})-(\d\d)-(\d\d)\s*$/,
                date = new Date(NaN), month,
                parts = isoExp.exec(dateString);

            if(parts) {
                month = +parts[2];
                date.setFullYear(parts[1], month - 1, parts[3]);
                if(month != date.getMonth() + 1) {
                    date.setTime(NaN);
                }
            }
            return date;
        },

        getUTCtime: function (time) {
            var dateSplit = /(\d{4})-(\d{2})-(\d{2})\s+(\d{2}):(\d{2}):(\d{2})/.exec(time);

            if (dateSplit.length == 7) {
                var date = new Date();
                date.setYear(dateSplit[1]);
                date.setMonth(dateSplit[2] - 1);
                date.setDate(dateSplit[3]);
                date.setHours(dateSplit[4]);
                date.setMinutes(dateSplit[5]);
                date.setSeconds(dateSplit[6]);

                return date.getTime();
            }else {
                return null;
            }
        }
    };
}();