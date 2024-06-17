app.initFont(["Microsoft YaHei","iconfont.ttf"]);
let win = new Win("我的窗口", 800, 600); //默认为透明窗口

let closeDiv = Div.newLTRB(715, 50, 750, 80);
closeDiv.setColor(0xFFAA66FF);
closeDiv.setFontFamily("iconfont.ttf");
closeDiv.setAlign(1, 1);
closeDiv.setText(0xE6E6);
closeDiv.setFontSize(14);
closeDiv.setTextColor(0xFFFFFFFF);
closeDiv.onMouseEnter(() => {
    closeDiv.setColor(0xFF662288);
    win.refresh();
});
closeDiv.onMouseLeave(() => {
    closeDiv.setColor(0xFFAA66FF);
    win.refresh();
});
closeDiv.onMouseDown(() => {
    win.close();
    console.log("down");
})


let minimizeDiv = Div.newLTRB(680, 50, 715, 80);
minimizeDiv.setColor(0xFFAA66FF);
minimizeDiv.setFontFamily("iconfont.ttf");
minimizeDiv.setAlign(1, 1);
minimizeDiv.setText(0xE6E7);
minimizeDiv.setFontSize(14);
minimizeDiv.setTextColor(0xFFFFFFFF);
minimizeDiv.onMouseEnter(() => {
    minimizeDiv.setColor(0xFF662288);
    win.refresh();
});
minimizeDiv.onMouseLeave(() => {
    minimizeDiv.setColor(0xFFAA66FF);
    win.refresh();
});
minimizeDiv.onMouseDown(() => {
    win.minimize();
})


let div = Div.newXYWH(50, 50, 700, 500);
div.setColor(0xFF663388);
div.setTextColor(0xFFAA3322)
div.setAlign(1, 1);
div.setFontSize(22);

let shadow = new Shadow();
shadow.setAmbientColor(0x33000000);
shadow.setSpotColor(0x00000000);
shadow.setShadowSize(30);
shadow.addRect(Rect.newXYWH(50, 50, 700, 500))

let img = Img.newXYWH(50, 50, 700, 350);
img.setSrc("img.png");
img.setAlpha(0.5)
win.addElement([shadow, div, img, closeDiv, minimizeDiv]);

let path = new Path();
path.addRect(Rect.newLTRB(50, 50, 680, 80));
win.setCaptionPath(path);

win.show();


globalThis.win = win;