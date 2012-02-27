create table output
(
    req_id int auto_increment primary key,
    req_by int,
    req_input text not null,
    output_type enum('TEXT','IMAGE','TEXT_AND_IMAGE','NO_OUTPUT','ERROR','OTHER') not null default 'NO_OUTPUT',
    output_ready boolean not null default false
);

