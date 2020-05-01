
direction = {
    none: 0,
    up: 1,
    down: 2,
    right: 3,
    left: 4
}

function setDirection(direction, playerIdx) {
    m.request({
        method: "POST",
        url: "/api/move/:key/:direction",
        params: {
            key: playerIdx,
            direction: direction
        }
    });
}
