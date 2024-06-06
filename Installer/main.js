﻿let win = new Win("我的窗口", 800, 800);
win.addEventListener("paint", () => {
    let paint = new Paint();

    //let path = new Path();
    //path.addEllipse(100, 100, 700, 700);
    //win.drawShadow(path, 0xFF888888, 0xFF00AA00);

    paint.setAntiAlias(true);
    paint.setStroke(false);
    //paint.setRadialShader(0, 1, 2, [0xFF008800, 0xFF2299FF]);
    paint.setColor(0xFF008800)
    win.drawEllipse(paint, 100, 100, 600, 600);

    paint.setColor(0xFF003388)
    win.drawRect(paint, 100, 100, 400, 400);
})

let path = new Path();
path.addRect(200, 200, 400, 400);
win.setCaptionPath(path);

win.show();
globalThis.win = win;