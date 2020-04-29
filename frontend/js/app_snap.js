var root = document.body

// parameters
var board_width = 800;

// Create Color Wheel
function hsl2rgb(h,s,l)
{
    let a=s*Math.min(l,1-l);
    let f= (n,k=(n+h/30)%12) => l - a*Math.max(Math.min(k-3,9-k,1),-1);
    return [f(0),f(8),f(4)];
}

// r,g,b are in [0-1], result e.g. #0812fa.
let rgb2hex = (r,g,b) =>
"#" + [r,g,b].map( x=>Math.round(x*255).toString(16).padStart(2,0)
).join('');

var nbr_color = 10
var nbr_color_row = 2
var color_wheel = []
color_wheel.push("#ffffff");
for (let j = 0; j < nbr_color_row; j++) {
    for (let i = 0; i < 360; i+= 360/nbr_color) {
        color_wheel.push(rgb2hex(...hsl2rgb(i, 1, j/(nbr_color_row-1)*(0.3) + 0.3)));
    }
}



var Client = {
    view: function (vnode) {
        return m("main", { class: "main" }, [
            m("h1", { class: "small-margin-bottom" }, "KOTH Snake"),
            m("div", { class: "pure-g" }, m("div", { class: "pure-u-1" }, m(Player))),
            m("button", {class: "pure-button button-small pure-button-primary", onclick: function() {BoardState.update_board();}}, "Update"),
            m("div", { class: "pure-g" }, m("div", { class: "pure-u-1" }, m(Board, {board_width: board_width, color_wheel: color_wheel})))
        ])
    }
}

m.mount(root, Client)

var sb = Snap("#board");
