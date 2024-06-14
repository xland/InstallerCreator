font.init(["Microsoft YaHei","iconfont.ttf"]);

let win = new Win("我的窗口", 800, 800);
let closeDiv = Div.newXYWH(720, 50, 30, 30);
closeDiv.setColor(0xFFAA66FF);
closeDiv.setFontFamily("iconfont.ttf");
closeDiv.setAlign(1, 1);
closeDiv.setText(0xE6E6);
closeDiv.setFontSize(14);
closeDiv.setTextColor(0xFF000000);

let div = Div.newXYWH(50, 50, 700, 700);
div.setColor(0xFF663388);
div.setTextColor(0xFFAA3322)
div.setFontFamily("Microsoft YaHei")
div.setText("繁荣昌盛！永世安宁！")
div.setAlign(1, 1);
div.setFontSize(22);

win.addElement([div,closeDiv]);

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