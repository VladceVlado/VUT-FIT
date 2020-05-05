create or replace trigger node_image_update
	after insert or update of image on nodes
	for each row
    declare
	si ORDSYS.SI_StillImage;
    begin
		if :NEW.image is not null and 1 = 2 then
			si := new SI_StillImage(:NEW.image.getContent());

			update nodes n set image_si = si,
				image_ac = SI_AverageColor(si)
			where n.id = :NEW.id;
		end if;
    end
;/

create or replace trigger node_auto_increment
	before insert on nodes
	for each row
    begin
	select node_id_seq.nextval
	into :NEW.id FROM DUAL;
    end
;/

create or replace trigger diagram_auto_increment
	before insert on diagrams
	for each row
    begin
	select diagram_id_seq.nextval
	into :new.id from dual;
    end 
;/

create or replace trigger node_relations_auto_increment
	before insert on node_relations
	for each row
    begin
	select node_relations_id_seq.nextval
	into :new.id from dual;
    end
;/

CREATE OR REPLACE TRIGGER node_add_trigger
	BEFORE INSERT
	ON nodes
	REFERENCING OLD as old
	NEW AS new
	FOR EACH ROW
	DECLARE
		n pls_integer;
	BEGIN
		SELECT COUNT(*)
			INTO n 
			FROM nodes 
			WHERE :new.diagram_id = diagram_id AND (SDO_RELATE(:new.shape, nodes.shape, 'mask=ANYINTERACT') = 'TRUE');
			
		IF  n > 0  THEN
			RAISE_APPLICATION_ERROR (-20000,'Objects are  overlapping!');
		END IF;

	END
;/

CREATE OR REPLACE TRIGGER node_delete_trigger
	BEFORE DELETE ON nodes
	FOR EACH ROW
	BEGIN
		DELETE node_relations 
		WHERE
			source_node_id = :old.id OR destination_node_id = :old.id;
	END
;/

CREATE OR REPLACE TRIGGER diagram_delete_trigger
	BEFORE DELETE ON diagrams
	FOR EACH ROW
	BEGIN
		DELETE nodes 
		WHERE
			diagram_id = :old.id;
	END
;/

CREATE OR REPLACE PROCEDURE merge_nodes(
	sourceNodeId IN nodes.id%TYPE,
	targetNodeId IN nodes.id%TYPE
	)
	AS
	target nodes%ROWTYPE;
	source nodes%ROWTYPE;
	BEGIN
		SELECT * INTO   target FROM   nodes WHERE  id = targetNodeId;
		SELECT * INTO   source FROM   nodes WHERE  id = sourceNodeId;
		UPDATE node_relations SET source_node_id = target.id WHERE source_node_id = source.id; 
		UPDATE node_relations SET destination_node_id = target.id WHERE destination_node_id = source.id;
		target.title := target.title||' '||source.text;
		target.text := target.text||' '||source.text;
		UPDATE nodes SET 
				text = target.text 
			WHERE id = target.id;
		DELETE nodes where id = source.id;
	END
;/

CREATE OR REPLACE TYPE int_array as TABLE OF INT
;/


CREATE OR REPLACE FUNCTION mbr_multiple
	( input_ids IN int_array)
	RETURN SDO_GEOMETRY
	IS
		final_geom SDO_GEOMETRY;
	BEGIN
		SELECT SDO_AGGR_MBR(shape) INTO final_geom FROM NODES where id in (SELECT * from table(input_ids));
		RETURN final_geom;
	END
;/      

CREATE OR REPLACE FUNCTION mbr_all
	( diag_id IN INT)
	RETURN SDO_GEOMETRY
	IS
		final_geom SDO_GEOMETRY;
	BEGIN
		SELECT SDO_AGGR_MBR(shape) INTO final_geom FROM NODES where diagram_id = diag_id;
		RETURN final_geom;
	END
;/   

CREATE OR REPLACE FUNCTION node_move
		( old_node_id IN INT, 
		new_shape IN SDO_GEOMETRY)
	RETURN INT
	IS
		num_of_overlaps INT;
		diag_id INT;
	BEGIN
		num_of_overlaps := 0;
		SELECT diagram_id INTO diag_id FROM nodes WHERE id = old_node_id;
		SELECT COUNT(*) INTO num_of_overlaps FROM nodes 
			WHERE id != old_node_id AND diagram_id = diag_id AND (SDO_RELATE(new_shape, shape, 'mask=ANYINTERACT') = 'TRUE');
		IF (num_of_overlaps != 0) THEN
			RAISE_APPLICATION_ERROR (-20000,'Objects are  overlapping!');
		ELSE
			RETURN 1;
		END IF;
	END
;/

CREATE OR REPLACE FUNCTION node_area_single
		(node_id IN INT)
	RETURN NUMBER
	IS
		output NUMBER;
	BEGIN
		SELECT SDO_GEOM.SDO_AREA(shape,0.005) INTO output FROM nodes WHERE id = node_id;
		RETURN output;
	END
;/

CREATE OR REPLACE FUNCTION node_area_all
		(diag_id IN INT)
	RETURN NUMBER
	IS
		output NUMBER;
	BEGIN
		SELECT SDO_GEOM.SDO_AREA(shape,0.005) INTO output FROM nodes WHERE diagram_id = diag_id;
		RETURN output;
	END
;/

