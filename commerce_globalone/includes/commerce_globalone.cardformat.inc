<?php
class CommerceGlobalonecardFormat{

  public $_paymentParams;
  public $_postHash;
  public $_terminal;
  public $_postDateTime;
  public $_xml;
  public $_normalizedPaymentParams;

  public function __construct($paymentParams,$terminal,$postHash,$postDateTime){
    $this->_paymentParams = $paymentParams;
    $this->_postHash = $postHash;
    $this->_terminal = $terminal;
    $this->_postDateTime = $postDateTime;
    $this->prepareCardXML();
  }

  private function prepareCardXML(){

    $xmlStructure = $this->readCardFields();

    $out = '<?xml version="'.$xmlStructure['XMLHeader']['version'].'" encoding="'.$xmlStructure['XMLHeader']['encoding'].'"?>';

    $out .= '<'.$xmlStructure['XMLEnclosureTag'].'>';

    $params = $this->preparePaymentParameter();

    foreach($params as $key=>$param):
      $tag = strtoupper($key);
    $out .= '<'.$tag.'>'.$param.'</'.$tag.'>';
    endforeach;
    $out .= '</'.$xmlStructure['XMLEnclosureTag'].'>';
    $this->_xml = $out;
    return $out;
  }

  protected function readCardFields() {
    return array(
      'XMLHeader' => array(
        'version' => '1.0',
        'encoding' => 'UTF-8',
      ),
      'XMLEnclosureTag' => 'SECURECARDREGISTRATION',
      'Fields' => array(
        array(
          'name' => 'MERCHANTREF',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'TERMINALID',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'DATETIME',
          'type' => 'datetime',
          'required' => TRUE,
          'format' => 'Y-m-dH:i:s:u',
          'form' => FALSE,
        ),
        array(
          'name' => 'NAME',
          'type' => 'text',
          'minlenght' => 16,
          'maxlenght' => 16,
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'DESCRIPTION',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'PERIODTYPE',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'LENGTH',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'CURRENCY',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'RECURRINGAMOUNT',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'INITIALAMOUNT',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'TYPE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'ONUPDATE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'ONDELETE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'HASH',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
      ),
    );
  }

  private function preparePaymentParameter(){
    $params = $this->_paymentParams;
    $out = array();

    $out['MERCHANTREF'] = $this->_terminal['terminal_id'].''.time();
    $out['TERMINALID'] = $this->_terminal['terminal_id'];
    //$out['AMOUNT'] = $params['AMOUNT'];
    $out['DATETIME'] = $this->_postDateTime;
    $out['CARDNUMBER'] = $this->cleanCardNumber($params['CARDNUMBER']);
    $out['CARDEXPIRY'] = $this->cleanExpiryDate($params['MONTH'],$params['YEAR']);
    $out['CARDTYPE'] = $params['CARDTYPE'];
    $out['CARDHOLDERNAME'] = $params['CARDHOLDERNAME'];
    $out['HASH'] = $this->_postHash;
    //$out['CURRENCY'] = $params['CURRENCY'];
    //$out['TERMINALTYPE'] = 2;
    //$out['TRANSACTIONTYPE'] = 7;
    //$out['CVV'] = $params['CVV'];
    // if (!empty($params['DESCRIPTION'])) {
    //   $out['DESCRIPTION'] = $params['DESCRIPTION'];
    // }

    $this->_normalizedPaymentParams = $out;

    return $out;
  }

  private function cleanCardNumber($cardNumber = ''){
    $cardNumber = str_replace('-' , '', $cardNumber);
    $cardNumber = str_replace(' ' , '', $cardNumber);

    return $cardNumber;
  }

  private function cleanExpiryDate($month = '',$year = ''){
    if(strlen($year)>2):
      $year = substr($year, 2,2);
    endif;

    $date = $month.$year;

    return $date;
  }

  public function getNormalizedPaymentParams(){
    return $this->_normalizedPaymentParams;
  }

  public function getXML(){
    return $this->_xml;
  }

}