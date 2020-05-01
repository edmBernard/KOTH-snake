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
      })
  },
  view: function (vnode) {
    return m("player.player-card.pure-g", [
      m(".player-index.pure-u-1-3", [
        m(".label", "Player Index"),
        m(".value", PlayerState.playerIdx)
      ]),
      m(".player-position.pure-u-1-3", [
        m(".label", "Position"),
        m(".pure-g", [
          m(".x-card.pure-u-1-2", [
            m(".label", "x: "),
            m(".value", PlayerState.position.x)
          ]),
          m(".y-card.pure-u-1-2", [
            m(".label", "y: "),
            m(".value", PlayerState.position.y)
          ]),
        ]),
      ]),
      m(".player-color.pure-u-1-3", [
        m(".label", "Color"),
        m(".value", PlayerState.color)
      ])
    ]);
  }
}
