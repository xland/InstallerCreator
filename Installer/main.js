﻿app.initFont(["Microsoft YaHei","iconfont.ttf"]);
let win = new Win("我的窗口", 800, 800);
let closeDiv = Div.newLTRB(715, 50, 750, 80);
closeDiv.setColor(0xFFAA66FF);
closeDiv.setFontFamily("iconfont.ttf");
closeDiv.setAlign(1, 1);
closeDiv.setText(0xE6E6);
closeDiv.setFontSize(14);
closeDiv.setTextColor(0xFFFFFFFF);
closeDiv.onMouseEnter(() => {
    closeDiv.setTextColor(0xFF000000);
    closeDiv.setColor(0xFF662288);
    win.refresh();
});
closeDiv.onMouseLeave(() => {
    closeDiv.setTextColor(0xFFFFFFFF);
    closeDiv.setColor(0xFFAA66FF);
    win.refresh();
});
closeDiv.onMouseDown(() => {
    win.close();
    console.log("down");
})
//win.addElement([shadow, div, img, closeDiv, minimizeDiv]);
win.addElement([closeDiv]);
win.show();
globalThis.win = win;