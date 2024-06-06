let win = new Win("我的窗口", 800, 800);
win.addEventListener("paint", () => {
    let paint = new Paint();
    paint.setAntiAlias(true);
    paint.setStroke(false);
    paint.setRadialShader(0, 1, 2, [0xFF008800, 0xFF2299FF]);
    win.drawEllipse(paint, 100, 100, 600, 600);
    let path = new Path();
    path.addEllipse(100, 100, 700, 700);
    win.drawShadow(path, 0xFF666666, 0x00000000);
})
win.show();
globalThis.win = win;