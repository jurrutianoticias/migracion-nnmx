<?php

class nnmxNodeCounterTableMigration extends Migration {
  public function __construct() {
    parent::__construct();
    // Human-friendly description of your migration process. Be as detailed as you like.
    $this->description = t('Migrate node_counter table "Statistics"');

    $this->dependencies = array('nnmxNodeBlog', 'nnmxNodeNota', 'nnmxNodeEdDigital', 'nnmxNodeVideo');

    $table_name = 'node_counter';
    $this->map = new MigrateSQLMap($this->machineName,
        array('nid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
              'description' => 'nodo id',
            )
          ),
        MigrateDestinationTable::getKeySchema($table_name)
      );

//query
$query = db_select(SOURCE_DATABASE . '.node_counter', 'f')
        ->fields('f', array('nid', 'totalcount', 'daycount', 'timestamp'));
        $this->source = new MigrateSourceSQL($query);
        $this->destination = new MigrateDestinationTable($table_name);


//map fields
        //fid uid filename URI  filemime  filesize status timestamp type
  $this->addFieldMapping('nid', 'nid');
  $this->addFieldMapping('totalcount', 'totalcount');
  $this->addFieldMapping('daycount', 'daycount');
  $this->addFieldMapping('timestamp', 'timestamp');
  }

   /*public function prepareRow($current_row) {
   }*/
}
