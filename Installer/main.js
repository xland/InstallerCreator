font.init(["Microsoft YaHei","iconfont.ttf"]);

let win = new Win("我的窗口", 800, 800);

let minimizeDiv = Div.newLTRB(680, 50, 715, 80);
minimizeDiv.setColor(0xFFAA66FF);
minimizeDiv.setFontFamily("iconfont.ttf");
minimizeDiv.setAlign(1, 1);
minimizeDiv.setText(0xE6E7);
minimizeDiv.setFontSize(14);
minimizeDiv.setTextColor(0xFF000000);
minimizeDiv.onMouseEnter(() => {
    minimizeDiv.setTextColor(0xFFFFFFFF);
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

let closeDiv = Div.newLTRB(715, 50, 750, 80);
closeDiv.setColor(0xFFAA66FF);
closeDiv.setFontFamily("iconfont.ttf");
closeDiv.setAlign(1, 1);
closeDiv.setText(0xE6E6);
closeDiv.setFontSize(14);
closeDiv.setTextColor(0xFF000000);
closeDiv.onMouseEnter(() => {
    closeDiv.setTextColor(0xFFFFFFFF);
    closeDiv.setColor(0xFF662288);
    win.refresh();
});
closeDiv.onMouseLeave(() => {
    closeDiv.setColor(0xFFAA66FF);
    win.refresh();
});
closeDiv.onMouseDown(() => {
    win.close();
})

let div = Div.newXYWH(50, 50, 700, 700);
div.setColor(0xFF663388);
div.setTextColor(0xFFAA3322)
div.setFontFamily("Microsoft YaHei")
div.setText("繁荣昌盛！永世安宁！")
div.setAlign(1, 1);
div.setFontSize(22);

let path = new Path();
path.addRect(Rect.newLTRB(50, 50, 680, 80));
win.setCaptionPath(path);

let shadow = new Shadow();
shadow.setAmbientColor(0x33000000);
shadow.setSpotColor(0x00000000);
shadow.setShadowSize(30);
shadow.addRect(Rect.newXYWH(50, 50, 700, 700))
win.addElement([shadow,div, closeDiv, minimizeDiv]);

//let rect = Rect.newXYWH(50, 50, 200, 200)
//win.addEventListener("paint", () => {
//    let paint = new Paint();
//    paint.setColor(0xFF663388)
//    let bodyRect = Rect.newXYWH(100, 100, 600, 600);
//    win.drawRect(paint, bodyRect);
//    paint.setAntiAlias(true);
//    paint.setStroke(false);
//    paint.setColor(0xFF660022)

//    win.drawText("测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试", paint, bodyRect, 0, 0);
//    //paint.setColor(0xFF008800)
//    //win.drawEllipse(paint, rect);
//})
//let path = new Path();
//path.addEllipse(rect);
//win.setCaptionPath(path);
//let i = 22;
//let id = win.setInterval(() => {
//    console.log(i);
//    i += 1;
//    if (i > 32) {
//        win.clearInterval(id);
//    }
//}, 1000);
win.show();
globalThis.win = win;