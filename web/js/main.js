
function Sequencer() {
  this._bpm = 120;
  this._matrixNotes = 16;
  this._matrixSteps = 16;
  this._buttonWidth = 40;
  this._buttonMargin = 10;

  this.insertDOMMatrix();
};

Sequencer.prototype.insertDOMMatrix = function() {
  for (var step = 0; step < this._matrixSteps; step++) {
    for (var note = 0; note < this._matrixNotes; note++) {
      var button = $('<div>&nbsp;</div>');

      // Style variations depending on column, position according to note
      // and step.
      button.addClass('button');
      if (step % 4 == 0) {  // Strong beat should be different style.
        button.addClass('strong');
      }
      var left = step * this._buttonWidth + step * this._buttonMargin;
      var bottom = note * this._buttonWidth + note * this._buttonMargin;
      button.css('width', this._buttonWidth + 'px');
      button.css('height', this._buttonWidth + 'px');
      button.css('left', left + 'px');
      button.css('bottom', bottom + 'px');

      // Data attributes need to be recorded for future use.
      button.attr('data-step', step);
      button.attr('data-note', note);
      $('#matrix').append(button);
    }
  }
  // Configure matrix container dimensions.
  var matrixHeight = this._matrixNotes * this._buttonWidth
      + (this._matrixNotes - 1) * this._buttonMargin;
  $('#matrix').css('height', matrixHeight);
};

$(document).ready(function() {
  window.sequencer = new Sequencer();
});
