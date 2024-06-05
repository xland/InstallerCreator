let win = new Win("我的窗口",800,800);
win.fillColor(0xFF00E3FD)
let paint = new Paint();
paint.setStroke(false);
paint.setStrokeWidth(6);
paint.setColor(0xFFAAE300);
win.drawRect(paint, 30, 30, 600, 600);
win.refresh();
globalThis.win = win;