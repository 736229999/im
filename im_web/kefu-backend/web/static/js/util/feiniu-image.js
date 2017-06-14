/*
 * 飞牛图片工具
 * ZhouXiang
 */
(function($) {
	window.feiniu || (window.feiniu = {});
	feiniu.config || (feiniu.config = {});
	feiniu.config.env || (feiniu.config.env = 'online');
	feiniu.image || (feiniu.image = {});
	feiniu.image.config || (feiniu.image.config = {});

	feiniu.image.config = $.extend({
		'newimage.local.download.host': 'http://10.202.184.107',
		'newimage.dev.download.host': 'http://10.202.184.107',
		'newimage.beta.download.host': 'http://10.202.249.218',
		'newimage.preview.download.host': 'http://img[16,17,18].fn-mart.com',
		'newimage.online.download.host': 'http://img[16,17,18].fn-mart.com',
		'oldscimage.local.download.host': 'http://imgsvr01.dev1.fn',
		'oldscimage.dev.download.host': 'http://imgsvr01.dev1.fn',
		'oldscimage.beta.download.host': 'http://imgsvr01.beta1.fn',
		'oldscimage.preview.download.host': 'http://img[16,17,18].fn-mart.com',
		'oldscimage.online.download.host': 'http://img[16,17,18].fn-mart.com',
		'oldzyimage.local.download.host': 'http://img.dev1.fn',
		'oldzyimage.dev.download.host': 'http://img.dev1.fn',
		'oldzyimage.beta.download.host': 'http://img.beta1.fn',
		'oldzyimage.preview.download.host': 'http://img[01,02,03,04,05,06].fn-mart.com',
		'oldzyimage.online.download.host': 'http://img[01,02,03,04,05,06].fn-mart.com'
	}, feiniu.image.config);

	// 获取图片完整路径，并做域名打散处理
	feiniu.image.getUrl = function(path) {
		return getImageInfo(path)[0];
	}

	// 获取图片缩略图完整路径，并做域名打散处理
	feiniu.image.getThumbUrl = function(path, width, height) {
		var imageInfo = getImageInfo(path);
		var url = imageInfo[0];
		var imageType = imageInfo[1];
		var thumbUrl = url;
		if (imageType != imageTypeUnknown) {
//			if (imageType == imageTypeScOld) {
//				thumbUrl = url.replace(/\/group\d+\/\w+\/\w+\/\w+\//, '/image/').replace(/(\.\w+)$/, '$1_' + width + 'x' + height + '$1');
//			} else {
			thumbUrl = url.replace(/(\.\w+)$/, "_" + width + "x" + height + "$1");
//			}
		}
		return thumbUrl;
	}

	var imageTypeNew = 1; // 新图片URL
	var imageTypeScOld = 2; // 原自营图片URL
	var imageTypeZyOld = 3; // 原商城图片URL
	var imageTypeUnknown = 4; // 未知URL
	var newMatcher = /^(http:\/\/.+?)?\/pic\/.+?\.\w+$/; // 新图片URL正则表达式
	var scOldMatcher = /^http:\/\/.+?\/group\d+\/.+?\.\w+$/; // 原商城图片URL正则表达式
	var zyOldMatcher = /^(http:\/\/.+?)?\/C\/.+?\.\w+$/; // 原自营图片URL正则表达式
	var thumbMatcher = /\d+x\d+\.\w+$/; // 缩略图正则表达式

	var newImageHosts, oldScImageHosts, oldZyImageHosts;
	var inited = false;

	function init() {
		var currentHref = window.location.href;
		var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

		if (currentDomain.indexOf('beta.fn') > -1)/**beta环境**/{
			feiniu.config.env = "beta";
		} else if (currentDomain.indexOf('dev.fn') > -1 ||currentDomain.indexOf('local.fn') > -1 )/**dev环境**/{
			feiniu.config.env = "dev";
		}

		var newImageDownloadHostExp = feiniu.image.config["newimage." + feiniu.config.env + ".download.host"];
		var oldScImageDownloadHostExp = feiniu.image.config["oldscimage." + feiniu.config.env + ".download.host"];
		var oldZyImageDownloadHostExp = feiniu.image.config["oldzyimage." + feiniu.config.env + ".download.host"];
		newImageHosts = splitHostExpress(newImageDownloadHostExp);
		oldScImageHosts = splitHostExpress(oldScImageDownloadHostExp);
		oldZyImageHosts = splitHostExpress(oldZyImageDownloadHostExp);
	}

	function splitHostExpress(hostExpress) {
		var hostList = [];
		var regex = /\[([\w,]+)\]/;
		var matcher = hostExpress.match(regex);
		if (matcher != null) {
			var strHashValues = matcher[1];
			var hashValues = strHashValues.split(",");
			$.each(hashValues, function(index, value) {
				var imageDownloadHost = hostExpress.replace(regex, value);
				hostList.push(imageDownloadHost);
			});
		} else {
			hostList.push(hostExpress);
		}
		return hostList;
	}

	function getHost(imageType) {
		var host = "";
		var list = null;
		if (imageType == imageTypeNew) {
			list = newImageHosts;
		} else if (imageType == imageTypeScOld) {
			list = oldScImageHosts;
		} else if (imageType == imageTypeZyOld) {
			list = oldZyImageHosts;
		}
		if (list != null) {
			var index = 0;
			if (list.length > 1) {
				index = Math.floor(Math.random() * list.length);
			}
			host = list[index];
			if (/\/$/.test(host)) {
				host = host.substring(0, host.length - 1);
			}
		}
		return host;
	}

	function getImageInfo(path) {
		if (!inited) {
			init();
			inited = true;
		}
		var url = path;
		var imageType = newMatcher.test(path) ? imageTypeNew : scOldMatcher.test(path) ? imageTypeScOld : zyOldMatcher.test(path) ? imageTypeZyOld : imageTypeUnknown;
		if (imageType != imageTypeUnknown && thumbMatcher.test(path)) { // 已做过缩略图URL转换的路径强制设为未知URL，不做转换处理
			imageType = imageTypeUnknown;
		}
		if (imageType != imageTypeUnknown) {
			var host = getHost(imageType);
			var isAbsolutePath = /^http:\/\//.test(path);
			if (isAbsolutePath) {
				url = path.replace(/http:\/\/.+?(?=\/)/, host);
			} else {
				if (/^\//.test(path)) {
					url = host + path;
				} else {
					url = host + "/" + path;
				}
			}
		}
		return [url, imageType];
	}
})(jQuery);