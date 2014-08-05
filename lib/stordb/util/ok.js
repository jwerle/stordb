var i = 0
  , expected = null
  ;
module.exports = function ok (message) {
  i += 1
  console.log('ok '+i+' '+ (message || '') )
}
module.exports.done = function done () {
  if (expected !== null && expected !== i) throw new Error('expected number of success conditions not met.')
  console.log('1..'+i)
}
module.exports.expect = function (int) {
  expected = int
}
