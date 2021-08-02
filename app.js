const { Client, Collection, Discord, MessageEmbed } = require('discord.js');
const bot = new Client();

const { prefix } = require(`./configs/config.json`);
[`aliases`, `commands`].forEach(x => bot[x] = new Collection());
["command", "events"].forEach(x => require(`./handlers/${x}`)(bot));

bot.on('message', message => {
  if (message.content == `<@${bot.user.id}>` || message.content == `<@!${bot.user.id}>`)
    return message.channel.send(`Hey there! My prefix is \`;\`!`);
  if(message.content == "rtu") {
    const embed = new MessageEmbed()
    .setThumbnail('http://www.thecolorapi.com/id?format=svg&hex=FF0000')

    message.channel.send({embed})
  }
})


const { TOKEN } = require('./configs/config.json');
bot.login(`${TOKEN}`);