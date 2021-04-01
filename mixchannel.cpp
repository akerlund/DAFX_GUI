#include "mixchannel.h"

MixChannel::MixChannel(int mix_id, QObject *parent) : QObject(parent)
{
  _id = mix_id;
  main_layout  = new QHBoxLayout();
  _head_layout = new QVBoxLayout();
  _body_layout = new QGridLayout();

  _lbl_pan  = new QLabel("PAN");
  _lbl_gain = new QLabel("GAIN");
  _lbl_freq = new QLabel("FREQ");
  _lbl_duty = new QLabel("DUTY");
  _lbl_wave = new QLabel("WAVE");
  _lbl_pan->setMaximumHeight(30);
  _lbl_gain->setMaximumHeight(30);
  _lbl_freq->setMaximumHeight(30);
  _lbl_duty->setMaximumHeight(30);
  _lbl_wave->setMaximumHeight(30);

  _dia_pan  = new QDial();
  _qle_pan  = new QLineEdit("0");
  _sli_gain = new QSlider();
  _sli_freq = new QSlider();
  _qle_gain = new QLineEdit("0");
  _qle_freq = new QLineEdit("0");
  _cmb_wave = new QComboBox();
  _sli_duty = new QSlider();
  _qle_duty = new QLineEdit("0");

  _dia_pan->setNotchesVisible(true);
  _dia_pan->setMaximumWidth(50);
  _dia_pan->setMaximumHeight(50);
  _sli_gain->setOrientation(Qt::Vertical);
  _sli_gain->setMaximumHeight(300);
  _sli_gain->setMaximumWidth(25);
  _sli_gain->setSliderPosition(50);
  _sli_freq->setOrientation(Qt::Vertical);
  _sli_freq->setMaximumHeight(300);
  _sli_freq->setMaximumWidth(25);
  _sli_gain->setSliderPosition(50);
  _qle_pan->setMaximumWidth(50);
  _qle_gain->setMaximumWidth(50);
  _qle_freq->setMaximumWidth(50);
  _qle_duty->setMaximumWidth(50);
  _cmb_wave->insertItem(0, "SQR");
  _cmb_wave->insertItem(1, "TRI");
  _cmb_wave->insertItem(2, "SAW");
  _cmb_wave->insertItem(3, "SIN");
  _cmb_wave->setMaximumWidth(50);
  _sli_duty->setMaximumWidth(50);
  _sli_duty->setOrientation(Qt::Horizontal);
  _sli_duty->setSliderPosition(50);

  _body_layout->addWidget(_lbl_gain, 0, 0);
  _body_layout->addWidget(_lbl_freq, 0, 1);
  _body_layout->addWidget(_sli_gain, 1, 0);
  _body_layout->addWidget(_sli_freq, 1, 1);
  _body_layout->addWidget(_qle_gain, 2, 0);
  _body_layout->addWidget(_qle_freq, 2, 1);
  _body_layout->addWidget(_lbl_wave, 3, 0);
  _body_layout->addWidget(_lbl_duty, 3, 1);
  _body_layout->addWidget(_cmb_wave, 4, 0);
  _body_layout->addWidget(_sli_duty, 4, 1);
  _body_layout->addWidget(_qle_duty, 5, 1);

  _head_layout->addWidget(_lbl_pan);
  _head_layout->addWidget(_dia_pan);
  _head_layout->addWidget(_qle_pan);
  _head_layout->addLayout(_body_layout);
  _head_layout->addStretch();

  main_layout->addLayout(_head_layout);
  main_layout->addStretch();

  connect(_dia_pan,  &QSlider::valueChanged,      this, &MixChannel::when_pan0_changed);
  connect(_qle_pan,  &QLineEdit::editingFinished, this, &MixChannel::when_pan1_changed);

  connect(_sli_gain, &QSlider::valueChanged,      this, &MixChannel::when_gain0_changed);
  connect(_qle_gain, &QLineEdit::editingFinished, this, &MixChannel::when_gain1_changed);

  connect(_sli_freq, &QSlider::valueChanged,      this, &MixChannel::when_freq0_changed);
  connect(_qle_freq, &QLineEdit::editingFinished, this, &MixChannel::when_freq1_changed);

  connect(_sli_duty, &QSlider::valueChanged,      this, &MixChannel::when_duty0_changed);
  connect(_qle_duty, &QLineEdit::editingFinished, this, &MixChannel::when_duty1_changed);


  _sli_gain->setValue(_id);
}

void MixChannel::when_pan0_changed(int value) {
  _qle_pan->setText(QString::number(value));
  emit pan_value_changed(_id, value);
}

void MixChannel::when_pan1_changed() {
  _dia_pan->setValue(_qle_pan->text().toInt());
  emit pan_value_changed(_id, _qle_pan->text().toInt());
}

void MixChannel::when_gain0_changed(int value) {
  _qle_gain->setText(QString::number(value));
  emit gain_value_changed(_id, value);
}

void MixChannel::when_gain1_changed() {
  _sli_gain->setValue(_qle_gain->text().toInt());
  emit gain_value_changed(_id, _qle_gain->text().toInt());
}

void MixChannel::when_freq0_changed(int value) {
  _qle_freq->setText(QString::number(value));
  emit freq_value_changed(_id, value);
}

void MixChannel::when_freq1_changed() {
  _sli_freq->setValue(_qle_freq->text().toInt());
  emit freq_value_changed(_id, _qle_freq->text().toInt());
}

void MixChannel::when_wave_changed() {

}

void MixChannel::when_duty0_changed(int value) {
  _qle_duty->setText(QString::number(value));
  emit duty_value_changed(_id, value);
}

void MixChannel::when_duty1_changed() {
  _sli_duty->setValue(_qle_duty->text().toInt());
  emit duty_value_changed(_id, _qle_duty->text().toInt());
}
