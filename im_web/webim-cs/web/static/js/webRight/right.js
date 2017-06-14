var TemplatePic =
    '<li class="hrefPic" style="float: left;"><a onclick="window.open($hrefUrl)"><img src="$picUrl" ></a></li>';
var TemplateLogistic =
    '<div class="track">' +
    '<div class="t_left"> <i class="$on"></i> <em class="$on"></em> </div>' +
    '<p>$detail<br>' +
    '$time</p>' +
    '</div>';

var TemplateQuestion =
    ' <h3 class="question QuestionAnswer" value="$questionId">$question</h3>' +
    '  <div class="p_answer QuestionAnswer">' +
    '  <div class="bg"> <em></em> <span>$answer</span> </div>' +
    ' </div>';

var TemplatePackageIndex =
    '<li class="$isCur packageIndex indexControl" value="$index">包裹$Show</li>';

function initSlide() {
    var Carousel = FNX.include('fnx/cmp/switchable/carousel');

    var slide = new Carousel({
        element: $('.carousel'),
        effect: 'scrollx',
        size: 4,
        step: 4,
        hasTriggers: false
    });
}
var serviceLog = {
    _env: "online",
    _tempVars: {
        orderUrl: "",
        title: "最近订单",
        orderNo: "服务查询",
        payType: "普通",
        price: "处理中",
        orderTime: "2018-09-19 20:10:12",
        orderStatus: "交易完成",
        packageDetailList: null,
        commonQustionsList: null
    },
    updateCodeToUI: function (opts, opt, codeString) {
        var self = this;
        $('#' + opts + ' .' + opt).remove();
        var askingPics = $('#' + opts);
        askingPics.append(codeString);
    },
    getGoodPicInfoCode: function (packageDetail) {
        var goodLen = packageDetail.goodPicInfoList.length;
        var toAppend = '';
        for (var i = 0; i < goodLen; i++) {
            var goodPicInfo = packageDetail.goodPicInfoList[i];
            var hrefUrl = "'" + goodPicInfo.hrefURL + "'";
            var picUrl = goodPicInfo.picURL;

            var tmp = "";
            tmp = TemplatePic;

            picUrl = feiniu.image.getThumbUrl(picUrl, 160, 160);

            var urlIndex = tmp.replace(/\$hrefUrl/g, hrefUrl.toString())
                .replace(/\$picUrl/g, picUrl.toString());

            toAppend += urlIndex;
        }
        return toAppend;
    },
    getLogisticCode: function (packageDetail) {
        var goodLen = packageDetail.logisticsDetailList.exp_info.length;
        var toAppend = '';
        for (var i = 0; i < goodLen; i++) {
            var logisticsDetail = packageDetail.logisticsDetailList.exp_info[i];
            var detail = logisticsDetail.statusDescUser;
            var time = logisticsDetail.hdInsDt;

            var tmp = "";
            tmp = TemplateLogistic;

            var urlIndex = tmp.replace(/\$detail/g, detail.toString())
                .replace(/\$time/g, time.toString());

            if (i == 0)
                urlIndex = urlIndex.replace(/\$on/g, "on");
            else
                urlIndex = urlIndex.replace(/\$on/g, "");

            toAppend += urlIndex;
        }
        return toAppend;
    },
    getCommenQuestionsCode: function (commonQustionsList) {
        var questionsLen = commonQustionsList.length;
        var toAppend = '';
        for (var i = 0; i < questionsLen; i++) {
            var commonQuestion = commonQustionsList[i];
            var question = commonQuestion.qustion;
            var answer = commonQuestion.answer;
            var questionId = commonQuestion.questionId;

            var tmp = "";
            tmp = TemplateQuestion;

            var urlIndex = tmp.replace(/\$questionId/g, questionId.toString())
                .replace(/\$question/g, question.toString())
                .replace(/\$answer/g, answer.toString());

            toAppend += urlIndex;
        }
        return toAppend;
    },
    getPackageIndexCode: function (total, index) {
        var toAppend = '';
        for (var i = 0; i < total; i++) {
            var tmp = TemplatePackageIndex;
            var indexShow = i + 1;
            var urlIndex = tmp.replace(/\$index/g, i.toString())
                .replace(/\$Show/g, indexShow.toString());

            if (i == index) {
                urlIndex = urlIndex.replace(/\$isCur/g, "cur");
            } else
                urlIndex = urlIndex.replace(/\$isCur/g, "");

            toAppend += urlIndex;
        }
        return toAppend;
    },
    setIndexPackageInfoToUI: function (index) {
        var self = this;
        var packageDetailList = self._tempVars.packageDetailList;
        var packageDetail = packageDetailList[index];
        var toAppendGoodPic = self.getGoodPicInfoCode(packageDetail);
        self.updateCodeToUI("AskingPictures", "hrefPic", toAppendGoodPic);
        var toAppendLogistic = self.getLogisticCode(packageDetail);
        self.updateCodeToUI("Logistics", "track", toAppendLogistic);

        if (packageDetailList.length <= 5)//包裹数
        {
            $('#packageControlPrev, #packageControlNext').hide();
        }

        var imgs = $('#AskingPictures li img');//每个包裹的物品数
        if (imgs.length <= 4) {
            $('#js_prev, #js_nexty').hide();
        }
    },
    flushParamsToUI: function () {
        var self = this;
        $("#J_store").text(self._tempVars.title);
        $("#orderNo").text(self._tempVars.orderNo);
        $("#payType").text(self._tempVars.payType);
        $("#price").text(self._tempVars.price);
        $("#orderTime").text(self._tempVars.orderTime);
        $("#orderStatus").text(self._tempVars.orderStatus);
        var packageDetailList = self._tempVars.packageDetailList;
        if (packageDetailList == null || packageDetailList.length == 0) {
            $("#package-area").hide();
        } else {
            self.setIndexPackageInfoToUI(0);
            var toAppendPackageIndex = self.getPackageIndexCode(self._tempVars.packageDetailList.length, 0);
            self.updateCodeToUI("packageIndexs", "packageIndex", toAppendPackageIndex);
        }
        var toAppendCommenQuestions = self.getCommenQuestionsCode(self._tempVars.commonQustionsList);
        self.updateCodeToUI("QuestionAnswers", "QuestionAnswer", toAppendCommenQuestions);
    },
    init: function () {
        this.initData();
        this.initUI();
        this.bindUI();
    },
    initData: function () {
        var self = this;
        $tempdata = $('tmpdata');
        var webRightResponse = $tempdata.data('webrightresponse') || [];
        self._tempVars = webRightResponse;
        $tempdata.remove();

        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('beta.fn') > -1 || currentDomain.indexOf('beta1.fn') > -1)/**beta环境**/{
            self._env = "beta";
        } else if (currentDomain.indexOf('dev.fn') > -1 || currentDomain.indexOf('local.fn') > -1
            || currentDomain.indexOf('dev1.fn') > -1)/**dev环境**/{
            self._env = "beta";
        } else {
            self._env = "online";
        }
    },
    initUI: function () {
        var self = this;
        self.flushParamsToUI();
        initSlide();
    },
    bindUI: function () {
        var self = this;

        $('.indexControl').click(function () {
            var index = this.value;

            $(this).addClass('cur').siblings().removeClass('cur');
            self.setIndexPackageInfoToUI(index)
        });

        $('#packageControlPrev').click(function () {
            var index = parseInt($('.cur.packageIndex').val()) - 1;
            if (index < 0 || index >= self._tempVars.packageDetailList.length) {
                return;
            }
            self.setIndexPackageInfoToUI(index)
        });

        $('#packageControlNext').click(function () {
            var index = parseInt($('.cur.packageIndex').val()) + 1;
            if (index < 0 || index >= self._tempVars.packageDetailList.length) {
                return;
            }
            self.setIndexPackageInfoToUI(index)
        });

        $('#orderNo').click(function () {
            window.open(self._tempVars.orderUrl)
        });

        $('.clickPic').click(function () {
            window.open(this.value);
        });

        $('.question').click(function () {
            $(this).next('.p_answer.QuestionAnswer').slideToggle();

            var questionId = this.getAttribute("value");
            $.post('../update-quick-phrase',
                {
                    "questionId": questionId
                },
                function (resultInfo) {
                    //不关心是否成功
                },
                'json');
        });

        $('#searchOrder').click(function () {
            if (self._env == "online") {
                window.open('https://home.feiniu.com/order/orderList');
            } else {
                window.open('https://home.beta1.fn/order/orderList');
            }
        });
        $('#searchGoodsReject').click(function () {
            if (self._env == "online") {
                window.open('http://sale.feiniu.com/help_center/hc-9.html');
            } else {
                window.open('http://sale.feiniu.com/help_center/hc-9.html');
            }
        });
        $('#searchCommonQuestions').click(function () {
            if (self._env == "online") {
                window.open('http://sale.feiniu.com/help_center/hc-10.html');
            } else {
                window.open('http://sale.feiniu.com/help_center/hc-10.html');
            }
        });
    }
};
$(function () {
    serviceLog.init();
});
