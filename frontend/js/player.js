var PlayerState = {
  playerIdx: 0,
  color: 0,
  position: {
    x: 0,
    y: 0
  }
}


var Player = {
  oninit: function(vnode) {
    m.request({
      method: "GET",
      url: "/api/register/Erwan/1",
      extract: function(xhr) {return xhr.responseText}
    })
    .then(function(result) {
        console.log(result);
        [PlayerState.playerIdx, PlayerState.position.x, PlayerState.position.y, PlayerState.color] = result.split(",");
    })
  },
  view: function(vnode) {
    return m("player", [
      m("h2",  { class: "small-margin-bottom" }, "Player Index : " +  PlayerState.playerIdx),
      m("div", "Position : " +  PlayerState.position.x + "," +  PlayerState.position.y),
      m("div", "Color : " +  PlayerState.color),
    ]);
  }
}
