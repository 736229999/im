/**
 * Author: morningking
 * Date: 2015/9/29
 * Contact: 243717042@qq.com
 */

var AlertUtil = {
    showError: function (content) {
        $.messager.alert('错误', '操作失败:' + content, 'error');
    },
    showInfo: function (content) {
        $.messager.alert('提醒', '操作成功:' + content, 'info');
    }
}
