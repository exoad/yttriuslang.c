const { MessageEmbed } = require('discord.js');
const config = require('../../configs/token.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
const colors = require('../../configs/colors.json');
module.exports = {
    config: {
        name: `invite`,
        category: '',
        description: '',
        aliases: [`invitebot`]
    },
    run: async (bot, message, args) => {
    try {
        const embed = new MessageEmbed()
        .setTitle("Invite Sci-Bot to your servers")
        .setDescription("Wanna invite `Sci-Bot` to your own server(s)? Check the links below!")
        .addField("Invite Link #1 (Admin)", "[Admin](https://discord.com/api/oauth2/authorize?client_id=834784133942607882&permissions=8&scope=bot)")
        .addField("Invite Link #2 (Primary Permissions)", "[Primary Permissions](https://discord.com/oauth2/authorize?client_id=834784133942607882&permissions=3154508918&scope=bot)")
        .addField("Invite Link #3 (Limited Permissions)", "[Limited Permissions](https://discord.com/api/oauth2/authorize?client_id=834784133942607882&permissions=201714752&scope=bot)")
        .setFooter("Thanks for inviting the bot!")

        message.channel.send({embed})
    } catch (err) {
        const embed = new MessageEmbed()
        .setTitle("Whoops, looks like something went wrong!")
        .setThumbnail(resource.aw_snap)
        .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands or use `"+config.prefix+"support` to join the support server!")
        .setFooter("Still facing issues? Join the support server!")
        message.channel.send({ embed }).then(m => {
            m.delete({ timeout: 5000 })
        });
    }
  }
  }