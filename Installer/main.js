let win = new Win("我的窗口", 800, 800);
win.addEventListener("paint", () => {
    win.fillColor(0x0000E3FD)
    let paint = new Paint();
    paint.setAntiAlias(true);
    paint.setStroke(false);
    paint.setStrokeWidth(6);
    paint.setColor(0xFFAAE300);
    paint.setRadialShader(0, 1, 2, [0xFF008800, 0xFF2299FF]);
    win.drawEllipse(paint, 100, 100, 600, 600);
})
win.show();
globalThis.win = win;