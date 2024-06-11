font.init(["Microsoft YaHei"]);
let win = new Win("我的窗口", 800, 800);
let rect = Rect.newXYWH(50, 50, 200, 200)
win.addEventListener("paint", () => {
    let paint = new Paint();
    paint.setColor(0xFF663388)
    let bodyRect = Rect.newXYWH(100, 100, 600, 600);
    win.drawRect(paint, bodyRect);
    paint.setAntiAlias(true);
    paint.setStroke(false);
    paint.setColor(0xFF660022)

    win.drawText("测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试", paint, bodyRect, 0, 0);
    //paint.setColor(0xFF008800)
    //win.drawEllipse(paint, rect);
})
let path = new Path();
path.addEllipse(rect);
win.setCaptionPath(path);
let i = 22;
let id = win.setInterval(() => {
    console.log(i);
    i += 1;
    if (i > 32) {
        win.clearInterval(id);
    }
}, 1000);
win.show();
globalThis.win = win;