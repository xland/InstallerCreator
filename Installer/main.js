font.init(["Microsoft YaHei"]);

let win = new Win("我的窗口", 800, 800);
let rect = Rect.newXYWH(50, 50, 200, 200)
win.addEventListener("paint", () => {
    let paint = new Paint();
    paint.setColor(0xFF663388)
    win.drawRect(paint, Rect.newXYWH(100, 100, 600, 600));

    //let path = new Path();
    //path.addEllipse(100, 100, 700, 700);
    //win.drawShadow(path, 0xFF888888, 0xFF00AA00);

    paint.setAntiAlias(true);
    paint.setStroke(false);
    //paint.setRadialShader(0, 1, 2, [0xFF008800, 0xFF2299FF]);
    paint.setColor(0xFF008800)
    win.drawEllipse(paint, rect);
    //win.drawEllipse(paint, 100, 100, 600, 600);

})

let path = new Path();
path.addEllipse(rect);
win.setCaptionPath(path);
//console.log(222);
//win.setTimeout(() => { console.log(111222); }, 1000);
//win.setTimeout(() => { console.log(111333); }, 900);
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