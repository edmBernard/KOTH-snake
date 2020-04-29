var root = document.body

// parameters
var board_size = 800;
var height;
var width;
var square_size;

var Board = {
    view: function(vnode) {
        return m("svg", {id: "board", width: board_size, height: board_size});
    }
}

var Client = {
    view: function(vnode) {
        return m("main", {class: "main"}, [
            m("h1", {class: "small-margin-bottom"}, "KOTH Snake"),
            m("div", {class: "pure-g"}, m("div", {class: "pure-u-1"}, m(Board)))
        ])
    }
}

m.mount(root, Client)

var sb = Snap("#board");

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
var nbr_color_row = 10
var color_wheel = []
color_wheel.push("#ffffff");
for (let j = 0; j < nbr_color_row; j++) {
    for (let i = 0; i < 360; i+= 360/nbr_color) {
        color_wheel.push(rgb2hex(...hsl2rgb(i, 1, j/(nbr_color_row-1)*(0.3) + 0.3)));
    }
}

m.request({
    method: "GET",
    url: "/api/map",
    extract: function(xhr) {return xhr.responseText}
})
.then(function(result) {
    console.log("toto");
    console.log(result);

    let res = result.split(";");
    let [width, height] = res[0].split(",");
    let board = res[1].split(",");
    let square_size = board_size / width

    console.log(width);
    console.log(height);

    for (let j = 0; j < height; j++) {
        for (let i = 0; i < width; i++) {
            let square = sb.rect(i * square_size, j * square_size, square_size, square_size);
            square.attr({fill: color_wheel[board[i + j*width]]});
            square.attr({stroke: "#cccccc", strokeWidth: 1});
        }
    }
})
