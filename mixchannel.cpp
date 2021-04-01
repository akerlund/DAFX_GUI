#include "mixchannel.h"

MixChannel::MixChannel(QObject *parent) : QObject(parent)
{
  main_layout = new QHBoxLayout();
  _head_layout  = new QVBoxLayout();
  _body_layout      = new QGridLayout();

  _lbl_pan  = new QLabel("PAN");
  _lbl_g    = new QLabel("GAIN");
  _lbl_f    = new QLabel("FREQ");
  _lbl_duty = new QLabel("DUTY");
  _lbl_wave = new QLabel("WAVE");
  _lbl_pan->setMaximumHeight(30);
  _lbl_g->setMaximumHeight(30);
  _lbl_f->setMaximumHeight(30);
  _lbl_duty->setMaximumHeight(30);
  _lbl_wave->setMaximumHeight(30);

  _dia_pan  = new QDial();
  _qle_p    = new QLineEdit("0");
  _sli_g    = new QSlider();
  _sli_f    = new QSlider();
  _qle_g    = new QLineEdit("0");
  _qle_f    = new QLineEdit("0");
  _cmb_wave = new QComboBox();
  _sli_duty = new QSlider();
  _qle_d    = new QLineEdit("0");

  _dia_pan->setNotchesVisible(true);
  _dia_pan->setMaximumWidth(50);
  _dia_pan->setMaximumHeight(50);
  _sli_g->setOrientation(Qt::Vertical);
  _sli_g->setMaximumHeight(300);
  _sli_g->setMaximumWidth(25);
  _sli_g->setSliderPosition(50);
  _sli_f->setOrientation(Qt::Vertical);
  _sli_f->setMaximumHeight(300);
  _sli_f->setMaximumWidth(25);
  _sli_g->setSliderPosition(50);
  _qle_p->setMaximumWidth(50);
  _qle_g->setMaximumWidth(50);
  _qle_f->setMaximumWidth(50);
  _qle_d->setMaximumWidth(50);
  _cmb_wave->insertItem(0, "SQR");
  _cmb_wave->insertItem(1, "TRI");
  _cmb_wave->insertItem(2, "SAW");
  _cmb_wave->insertItem(3, "SIN");
  _cmb_wave->setMaximumWidth(50);
  _sli_duty->setMaximumWidth(50);
  _sli_duty->setOrientation(Qt::Horizontal);
  _sli_duty->setSliderPosition(50);

  _body_layout->addWidget(_lbl_g,    0, 0);
  _body_layout->addWidget(_lbl_f,    0, 1);
  _body_layout->addWidget(_sli_g,    1, 0);
  _body_layout->addWidget(_sli_f,    1, 1);
  _body_layout->addWidget(_qle_g,    2, 0);
  _body_layout->addWidget(_qle_f,    2, 1);
  _body_layout->addWidget(_lbl_wave, 3, 0);
  _body_layout->addWidget(_lbl_duty, 3, 1);
  _body_layout->addWidget(_cmb_wave, 4, 0);
  _body_layout->addWidget(_sli_duty, 4, 1);
  _body_layout->addWidget(_qle_d,    5, 1);

  _head_layout->addWidget(_lbl_pan);
  _head_layout->addWidget(_dia_pan);
  _head_layout->addWidget(_qle_p);
  _head_layout->addLayout(_body_layout);
  _head_layout->addStretch();

  main_layout->addLayout(_head_layout);
  main_layout->addStretch();

  connect(_dia_pan, &QSlider::valueChanged, this, &MixChannel::when_pan0_changed);
  connect(_qle_p,   &QLineEdit::editingFinished, this, &MixChannel::when_pan1_changed);

  connect(_sli_g,   &QSlider::valueChanged, this, &MixChannel::when_gain0_changed);
  connect(_qle_g,   &QLineEdit::editingFinished, this, &MixChannel::when_gain1_changed);

  connect(_sli_f,   &QSlider::valueChanged, this, &MixChannel::when_freq0_changed);
  connect(_qle_f,   &QLineEdit::editingFinished, this, &MixChannel::when_freq1_changed);

  connect(_sli_duty, &QSlider::valueChanged, this, &MixChannel::when_duty0_changed);
  connect(_qle_d,    &QLineEdit::editingFinished, this, &MixChannel::when_duty1_changed);
}

void MixChannel::when_pan0_changed(int value) {
  _qle_p->setText(QString::number(value));
}

void MixChannel::when_pan1_changed() {
  _dia_pan->setValue(_qle_p->text().toInt());
}

void MixChannel::when_gain0_changed() {
  _qle_g->setText(QString::number(_sli_g->value()));
}

void MixChannel::when_gain1_changed() {
  _sli_g->setValue(_qle_g->text().toInt());
}

void MixChannel::when_freq0_changed() {
  _qle_f->setText(QString::number(_sli_f->value()));
}

void MixChannel::when_freq1_changed() {
  _sli_f->setValue(_qle_f->text().toInt());
}

void MixChannel::when_wave_changed() {

}

void MixChannel::when_duty0_changed() {
  _qle_d->setText(QString::number(_sli_duty->value()));
}

void MixChannel::when_duty1_changed() {
  _sli_duty->setValue(_qle_d->text().toInt());
}
