var BoardState = {
  squareSize: 0,
  size: {
    width: 0,
    height: 0
  },
  dimension: {
    width: 0,
    height: 0
  },
  color_wheel: [],
  square_list: [],
  update_board: function () {
    console.log("update");
    m.request({
      method: "GET",
      url: "/api/map/buffer",
      extract: function (xhr) { return xhr.responseText }
    })
      .then(function (result) {
        let board = result.split(",");
        for (let j = 0; j < BoardState.dimension.height; j++) {
          for (let i = 0; i < BoardState.dimension.width; i++) {
            let squareIdx = i + j * BoardState.dimension.width;
            let square = BoardState.square_list[squareIdx];
            square.attr({ fill: BoardState.color_wheel[board[squareIdx]] });
            square.attr({ stroke: "#cccccc", strokeWidth: 1 });
          }
        }
        console.log("update done");
      })
  },
}


var Board = {
  oninit: function (vnode) {
    BoardState.size.width = vnode.attrs.board_width;
    BoardState.color_wheel = vnode.attrs.color_wheel;

    m.request({
      method: "GET",
      url: "/api/map/size",
      extract: function (xhr) { return xhr.responseText }
    })
      .then(function (result) {
        let [width, height] = result.split(",");

        BoardState.dimension = {
          width: width,
          height: height
        };
        BoardState.squareSize = BoardState.size.width / width
        BoardState.size.height = height * BoardState.squareSize;
      });
  },
  oncreate: function (vnode) {
    var sb = Snap(vnode.dom);

    m.request({
      method: "GET",
      url: "/api/map/buffer",
      extract: function (xhr) { return xhr.responseText }
    })
      .then(function (result) {
        let board = result.split(",");

        for (let j = 0; j < BoardState.dimension.height; j++) {
          for (let i = 0; i < BoardState.dimension.width; i++) {
            let square = sb.rect(i * BoardState.squareSize, j * BoardState.squareSize, BoardState.squareSize, BoardState.squareSize);
            square.attr({ fill: BoardState.color_wheel[board[i + j * BoardState.dimension.width]] });
            square.attr({ stroke: "#cccccc", strokeWidth: 1 });
            BoardState.square_list.push(square);
          }
        }
      })
  },
  view: function (vnode) {
    return m("svg#board", { width: BoardState.size.width, height: BoardState.size.height });
  }
}