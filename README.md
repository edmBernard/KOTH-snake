# KoTH-snake

Idea inspired by [gitland](https://github.com/programical/gitland) a game crontolled by github commit.
I create a small server to implement a similar game logic but without github.
The goal is to allow user to create bot that play the game. And in the futur be able to made a [King of the hill](https://codegolf.stackexchange.com/questions/tagged/king-of-the-hill) game with bot ranking.


## Compilation, Tests and Documentations

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

KoTH-snake depend on:
- [uWebSocket](https://github.com/uNetworking/uWebSockets): Websocket and http server
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library
- [Catch2](https://github.com/catchorg/Catch2): A modern, C++-native, header-only, test framework for unit-tests

```
./vcpkg install uWebSocket cxxopts fmt Catch2
```

### Compile , Test

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
make
```

the server executable is named `koth-snake-server`

## API

To be able to interact with the server there is few API implemented :

* `GET` : `/api/register/:name/:color` : Register a new player with its color
    * `curl http://<my-server-address>/api/register/XXXX/Y`
* `GET` : `/api/status/:key` : Get status (color, coordinate) of a player
    * `curl http://<my-server-address>/api/status/XXXX`
* `POST` : `/api/move/:key/:direction` : Send a new direction order to the server for this player
    * `curl -d "" http://<my-server-address>/api/move/XXXX/Y`
* `GET` : `/api/map/buffer` : Get the colored map as a vectorized image
    * `curl http://<my-server-address>/api/map/buffer`
* `GET` : `/api/map/size` : Get the map size
    * `curl http://<my-server-address>/api/map/size`
* `GET` : `/api/players/buffer` : Get the map indicating all player positions
    * `curl http://<my-server-address>/api/players/buffer`

## Disclaimer

It's a toy project. Mainly used to learn uwebsocket library.
I also try to make a more data oriented design.
So if you spot error, improvement comments are welcome.
