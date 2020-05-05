
create table diagrams (
	id integer not null,
	name varchar2(128) not null,
	author_name varchar(128),
	author_email varchar2(128),
	last_modified timestamp not null,
	constraint pk_diagram primary key (id)
);

create table nodes (
	id integer not null,
	title varchar2(128), 
	text varchar2(512),  
	color integer not null, 
	image ORDSYS.ORDImage,
	image_si ORDSYS.SI_StillImage,
	image_ac ORDSYS.SI_AverageColor,
	audio ORDSYS.ORDAudio, 
	shape SDO_GEOMETRY not null,
	diagram_id integer not null,
	constraint pk_node primary key (id),
	constraint fk_diagram
		foreign key (diagram_id)
		references diagrams (id)
);

create table node_relations (
	id integer not null,
	source_node_id integer not null,
	destination_node_id integer not null,
	constraint pk_node_relation primary key (id),
	constraint fk_node_source
		foreign key (source_node_id)
		references nodes (id),
	constraint fk_node_destination
		foreign key (destination_node_id)
		references nodes (id)
);

delete from user_sdo_geom_metadata where
	table_name = 'NODES' and column_name = 'SHAPE';

insert into user_sdo_geom_metadata values (
    'NODES', 'SHAPE',
    SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', 0, 500, 0.1), SDO_DIM_ELEMENT('Y', 0, 500, 0.1)),
    NULL
);

create index shape_idx on nodes (shape)
   INDEXTYPE is MDSYS.SPATIAL_INDEX;

create sequence node_id_seq;

create sequence diagram_id_seq;

create sequence node_relations_id_seq;