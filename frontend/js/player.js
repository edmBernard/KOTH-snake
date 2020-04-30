var PlayerState = {
  playerIdx: 0,
  color: 0,
  position: {
    x: 0,
    y: 0
  }
}


var Player = {
  oninit: function (vnode) {
    m.request({
      method: "GET",
      url: "/api/register/Erwan/1",
      extract: function (xhr) { return xhr.responseText }
    })
      .then(function (result) {
        [PlayerState.playerIdx, PlayerState.position.x, PlayerState.position.y, PlayerState.color] = result.split(",");

        setInterval(function () {

          m.request({
            method: "POST",
            url: "/api/move/:key/:dir",
            params: { key: PlayerState.playerIdx, dir: 1 }
          });
        }, 10000);
      })
  },
  view: function (vnode) {
    return m("player", [
      m("h2", { class: "small-margin-bottom" }, "Player Index : " + PlayerState.playerIdx),
      m("div", "Position : " + PlayerState.position.x + "," + PlayerState.position.y),
      m("div", "Color : " + PlayerState.color),
    ]);
  }
}
