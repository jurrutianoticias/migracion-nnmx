<?php

class nnmxFileTableMigration extends Migration {
  public function __construct() {
    parent::__construct();
    // Human-friendly description of your migration process. Be as detailed as you like.
    $this->description = t('Migrate table Files');

    $this->dependencies = array('nnmxUser');

    $table_name = 'file_managed';
    $this->map = new MigrateSQLMap($this->machineName,
        array('fid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
              'description' => 'file D6 unique id',
            )
          ),
        MigrateDestinationTable::getKeySchema($table_name)
      );

//query
$query = db_select(SOURCE_DATABASE . '.files', 'f')
        ->fields('f', array('fid', 'uid', 'filename', 'filepath', 'filemime', 'filesize', 'status', 'timestamp'))
        ->condition('f.filemime', 'application/x-shockwave-flash', '<>')
        ->condition('f.filemime', 'video/x-flv', '<>')
        ->condition('f.filemime', 'application/vnd.google-earth.kmz', '<>');
        $this->source = new MigrateSourceSQL($query);
        $this->destination = new MigrateDestinationTable($table_name);


//map fields
        //fid uid filename URI  filemime  filesize status timestamp type
  $this->addFieldMapping('fid', 'fid');
  $this->addFieldMapping('uid', 'uid');
  $this->addFieldMapping('filename', 'filename');
  $this->addFieldMapping('uri', 'filepath');
  $this->addFieldMapping('filemime', 'filemime');
  $this->addFieldMapping('filesize', 'filesize');
  $this->addFieldMapping('status', 'status');
  $this->addFieldMapping('timestamp', 'timestamp');
  $this->addFieldMapping('type')->defaultValue('image');
  }

   public function prepareRow($current_row) {
    $current_row->filepath = str_replace('sites/default/files/', 'public://', $current_row->filepath);
   }
}
