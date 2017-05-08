# BSWebServer
A Simple HTTP Server with Qt for Computer Networking Course Project.

## Feature
- Response GET request
- Response MIME type estimated with file extension <br/>(currently support `*.html, *.htm,  *.txt, *.css, *.js`, `*.json, *.pdf, *.xml`, `*.jpeg, *.jpg, *.png, *.gif, *.tiff, *.tif, *.ico, *.mp3, *.mp4`)
- Multithread processing
- Cache control
- Keep-Alive Connection
- Response 404 error and server internal error
- Partical content response (Response `Content-Range` Header)
- GUI log viewer with log file exporting support

## Todo
- Estimate response file type with `libmagic` instead of file extension
- CGI support
- Response POST request

## Build
Build with Qt Creator
