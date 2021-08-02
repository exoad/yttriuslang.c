'use strict'

const uniqueRandomArray = require('unique-random-array')
const xhr = require('request')
const randomCache = {}

function storeResults (images, subreddit) {
  const getRandomImage = uniqueRandomArray(images)

  randomCache[subreddit] = getRandomImage
  return getRandomImage
}

function randomFrog (subreddit, cacheURL) {
  subreddit = (typeof subreddit === 'string' && subreddit.length !== 0) ? subreddit : 'frogs'

  if (randomCache[subreddit]) {
    return Promise.resolve(randomCache[subreddit]())
  }

  var url = typeof cacheURL === 'string' ? cacheURL : `https://imgur.com/r/${subreddit}/hot.json`

  var p = new Promise(function (resolve, reject) {
    xhr.get(url, {json: true}, function (err, resp, body) {
      if (err) reject(err)
      resolve(body.data)
    })
  })
  return p.then(function (data) { return storeResults(data, subreddit) })
        .then(function (getRandomImage) { return getRandomImage() })
        .catch(function (err) { throw err })
}

function callback (subreddit, cb, cacheURL) {
  randomFrog(subreddit, cacheURL)
        .then(function (url) { return cb(null, url) })
        .catch(function (err) { return cb(err) })
}

// subreddit is optional
// callback support is provided for a training exercise
module.exports = function (subreddit, cb, cacheURL) {
  if (typeof cb === 'function') {
    callback(subreddit, cb, cacheURL)
  } else if (typeof subreddit === 'function') {
    callback(null, subreddit, cacheURL)
  } else {
    return randomFrog(subreddit, cacheURL)
  }
}
